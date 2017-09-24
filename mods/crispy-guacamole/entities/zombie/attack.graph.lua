script {
  links = {
    {
      1,
      1,
      6,
      1,
    },
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
  nodes = {
    {
      name = 'components/attackcomponent',
    },
    {
      name = 'number',
      loadArguments = {
        2,
      },
    },
    {
      name = 'number',
      loadArguments = {
        1,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        true,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        false,
      },
    },
    {
      name = 'script/scriptoutput',
      loadArguments = {
        {
          7,
          'Table',
        },
      },
    },
  },
}