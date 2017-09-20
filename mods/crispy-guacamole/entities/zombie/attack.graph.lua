script {
  nodes = {
    {
      name = 'components/attackcomponent',
    },
    {
      initArguments = {
        1,
      },
      name = 'number',
    },
    {
      initArguments = {
        1,
      },
      name = 'number',
    },
    {
      initArguments = {
        true,
      },
      name = 'bool',
    },
    {
      initArguments = {
        false,
      },
      name = 'bool',
    },
  },
  links = {
    {
      2,
      1,
      1,
      1,
    },
    {
      3,
      1,
      1,
      2,
    },
    {
      4,
      1,
      1,
      3,
    },
    {
      5,
      1,
      1,
      4,
    },
  },
}